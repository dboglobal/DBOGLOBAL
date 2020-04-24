CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2297;
	title = 229702;

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
				conv = 229709;
				ctype = 1;
				idx = 6311206;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 229708;
				gtype = 1;
				area = 229701;
				goal = 229704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 229705;
				stype = 1;
				taid = 1;
				title = 229702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 229714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 229701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3181305;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 229708;
			gtype = 1;
			oklnk = 2;
			area = 229701;
			goal = 229704;
			sort = 229705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 229702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 229707;
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
				npcidx = "6311206;";
			}
			CDboTSCheckClrQst
			{
				and = "2296;";
				flink = 1;
				flinknextqid = "2298;";
				not = 0;
			}
		}
	}
}

