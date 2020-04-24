CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 1;
	tid = 6906;
	title = 690602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 690608;
			gtype = 1;
			oklnk = 2;
			area = 690601;
			goal = 690604;
			sort = 690605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 690602;
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
			desc = 690614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 690601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5523101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActPortal
			{
				dx = "-0.445000";
				px = "3896.375977";
				widx = 1;
				dy = "0.000000";
				py = "0.000000";
				taid = 2;
				type = 1;
				dz = "0.895000";
				pz = "2945.270996";
			}
			CDboTSActRegQInfo
			{
				cont = 690608;
				gtype = 1;
				area = 690601;
				goal = 690604;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 690605;
				stype = 128;
				taid = 1;
				title = 690602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 690607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "6907;6908;";
				not = 0;
			}
			CDboTSClickSideIcon
			{
				sitype = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 1;
			}
		}
	}
}

