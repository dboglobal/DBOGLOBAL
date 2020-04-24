CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6882;
	title = 688202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 688207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 15;
			}
			CDboTSClickNPC
			{
				npcidx = "3185101;";
			}
			CDboTSCheckClrQst
			{
				and = "6879;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSSkillUse
			{
				idx = 20912;
				rp = 0;
				stype = 4;
				tidx = -1;
				ttype = 2;
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
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 688209;
				ctype = 1;
				idx = 3185101;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 430002;
				stype0 = 1;
				taid = 3;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 688208;
				gtype = 1;
				area = 688201;
				goal = 688204;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 688205;
				stype = 64;
				taid = 1;
				title = 688202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 688214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 688201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3185101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 688208;
			gtype = 1;
			oklnk = 2;
			area = 688201;
			goal = 688204;
			sort = 688205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 688202;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 430002;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 688207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

