CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2187;
	title = 218702;

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
				conv = 218709;
				ctype = 1;
				idx = 3614103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 218708;
				gtype = 1;
				area = 218701;
				goal = 218704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 218705;
				stype = 2;
				taid = 1;
				title = 218702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 218707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "3614103;";
			}
			CDboTSCheckClrQst
			{
				and = "2186;";
				flink = 1;
				flinknextqid = "2188;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 218708;
			gtype = 1;
			oklnk = 2;
			area = 218701;
			goal = 218704;
			sort = 218705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 218702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 218714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 218701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8552101;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

