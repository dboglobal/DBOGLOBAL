CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2307;
	title = 230702;

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
				conv = 230709;
				ctype = 1;
				idx = 1501104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 230708;
				gtype = 1;
				area = 230701;
				goal = 230704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 230705;
				stype = 1;
				taid = 1;
				title = 230702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 230714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 230701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5591109;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 230707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "1501104;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2308;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 230708;
			gtype = 1;
			oklnk = 2;
			area = 230701;
			goal = 230704;
			sort = 230705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 230702;
		}
	}
}

