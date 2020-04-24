CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2348;
	title = 234802;

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
				conv = 234809;
				ctype = 1;
				idx = 2981202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 234808;
				gtype = 1;
				area = 234801;
				goal = 234804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 234805;
				stype = 1;
				taid = 1;
				title = 234802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 234814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 234801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5064102;";
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
			cont = 234808;
			gtype = 1;
			oklnk = 2;
			area = 234801;
			goal = 234804;
			sort = 234805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 234802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 234807;
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
				npcidx = "2981202;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2349;";
				not = 0;
			}
		}
	}
}

