CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1349;
	title = 134902;

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
				conv = 134909;
				ctype = 1;
				idx = 5313101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 134908;
				gtype = 1;
				area = 134901;
				goal = 134904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 134905;
				stype = 2;
				taid = 1;
				title = 134902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 134907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 43;
			}
			CDboTSClickNPC
			{
				npcidx = "5313101;";
			}
			CDboTSCheckClrQst
			{
				and = "1326;";
				flink = 1;
				flinknextqid = "1350;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 134908;
			gtype = 1;
			oklnk = 2;
			area = 134901;
			goal = 134904;
			sort = 134905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 134902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 134914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 134901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5313201;";
			}
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
	}
}

