CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2178;
	title = 217802;

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
				conv = 217809;
				ctype = 1;
				idx = 3031406;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 217808;
				gtype = 1;
				area = 217801;
				goal = 217804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 217805;
				stype = 1;
				taid = 1;
				title = 217802;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 217814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 217801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3031305;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 217808;
			gtype = 1;
			oklnk = 2;
			area = 217801;
			goal = 217804;
			sort = 217805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 217802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 217807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSCheckClrQst
			{
				and = "2176;";
				flink = 1;
				flinknextqid = "2179;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3031406;";
			}
		}
	}
}

