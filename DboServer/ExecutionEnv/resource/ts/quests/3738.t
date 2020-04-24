CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3738;
	title = 373802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 373809;
				ctype = 1;
				idx = 5591110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 373808;
				gtype = 1;
				area = 373801;
				goal = 373804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 373805;
				stype = 2;
				taid = 1;
				title = 373802;
			}
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
			cont = 373808;
			gtype = 1;
			oklnk = 2;
			area = 373801;
			goal = 373804;
			sort = 373805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 373802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 373814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 373801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5591212;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 373807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "5591110;";
			}
			CDboTSCheckClrQst
			{
				and = "3737;";
				flink = 0;
				flinknextqid = "3739;";
				not = 0;
			}
		}
	}
}

