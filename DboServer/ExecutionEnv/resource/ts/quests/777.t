CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 777;
	title = 77702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 77709;
				ctype = 1;
				idx = 3332203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 77708;
				gtype = 1;
				area = 77701;
				goal = 77704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 77705;
				stype = 2;
				taid = 1;
				title = 77702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 77714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 77701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1311204;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 77708;
			gtype = 1;
			oklnk = 2;
			area = 77701;
			goal = 77704;
			sort = 77705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 77702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 77707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 26;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "778;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3332203;";
			}
		}
	}
}

