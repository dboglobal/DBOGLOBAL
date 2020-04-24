CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2329;
	title = 232902;

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
				conv = 232909;
				ctype = 1;
				idx = 7631102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 232908;
				gtype = 1;
				area = 232901;
				goal = 232904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 232905;
				stype = 2;
				taid = 1;
				title = 232902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 232907;
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
				npcidx = "7631102;";
			}
			CDboTSCheckClrQst
			{
				and = "2328;";
				flink = 1;
				flinknextqid = "2330;";
				not = 0;
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
			cont = 232908;
			gtype = 1;
			oklnk = 2;
			area = 232901;
			goal = 232904;
			sort = 232905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 232902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 232914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 232901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5591109;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

