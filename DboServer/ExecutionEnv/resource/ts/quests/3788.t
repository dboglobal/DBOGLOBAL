CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 3788;
	title = 378802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPortal
			{
				dx = "0.830000";
				px = "-2188.709961";
				widx = 15;
				dy = "0.000000";
				py = "-118.580002";
				taid = 1;
				type = 1;
				dz = "0.560000";
				pz = "-3988.350098";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 378807;
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
				npcidx = "3481103;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "3701;";
				not = 0;
			}
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
			cont = 378808;
			gtype = 1;
			oklnk = 2;
			area = 378801;
			goal = 378804;
			sort = 378805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 378802;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 378809;
				ctype = 1;
				idx = 3481103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 378808;
				gtype = 1;
				area = 378801;
				goal = 378804;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 378805;
				stype = 2;
				taid = 1;
				title = 378802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 378814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 378801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3481101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "3481103;";
			}
		}
	}
}

