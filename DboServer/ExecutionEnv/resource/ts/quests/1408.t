CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1408;
	title = 140802;

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
				conv = 140809;
				ctype = 1;
				idx = 2802101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 140808;
				gtype = 2;
				area = 140801;
				goal = 140804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 140805;
				stype = 2;
				taid = 1;
				title = 140802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 140814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 140801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1352101;";
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
			cont = 140808;
			gtype = 2;
			oklnk = 2;
			area = 140801;
			goal = 140804;
			sort = 140805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 140802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 140807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSClickNPC
			{
				npcidx = "2802101;";
			}
			CDboTSCheckClrQst
			{
				and = "1407;";
				flink = 1;
				flinknextqid = "1409;";
				not = 0;
			}
		}
	}
}

