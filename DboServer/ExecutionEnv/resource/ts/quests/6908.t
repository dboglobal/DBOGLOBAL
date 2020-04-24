CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 1;
	tid = 6908;
	title = 690802;

	CNtlTSGroup
	{
		gid = 0;

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
			cont = 690808;
			gtype = 1;
			oklnk = 2;
			area = 690801;
			goal = 690804;
			sort = 690805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 690802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 690807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				and = "6906;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5523101;";
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 31;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 690808;
				area = 690801;
				goal = 690804;
				m0fz = "3209.668945";
				m0widx = 1;
				scitem = -1;
				sort = 690805;
				stype = 128;
				m0fy = "-42.991001";
				m0pat = 2;
				grade = -1;
				m0fx = "3975.560059";
				m0ttip = 690815;
				rwd = 100;
				taid = 1;
				title = 690802;
				gtype = 1;
			}
			CDboTSActPortal
			{
				dx = "0.997000";
				px = "3905.352051";
				widx = 1;
				dy = "0.000000";
				py = "0.000000";
				taid = 2;
				type = 1;
				dz = "-0.084000";
				pz = "3212.200928";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 690814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 690801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5523103;";
			}
		}
	}
}

