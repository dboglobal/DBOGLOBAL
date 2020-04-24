CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2330;
	title = 233002;

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
				conv = 233009;
				ctype = 1;
				idx = 5591109;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 233008;
				gtype = 1;
				area = 233001;
				goal = 233004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 233005;
				stype = 2;
				taid = 1;
				title = 233002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 233007;
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
				npcidx = "5591109;";
			}
			CDboTSCheckClrQst
			{
				and = "2329;";
				flink = 1;
				flinknextqid = "2331;";
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
			cont = 233008;
			gtype = 1;
			oklnk = 2;
			area = 233001;
			goal = 233004;
			sort = 233005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 233002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 233014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 233001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1501104;";
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

