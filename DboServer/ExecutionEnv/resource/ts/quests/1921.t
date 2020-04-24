CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 1921;
	title = 192102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActDir
			{
				idx = 1092;
				rng = "30.000000";
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "5;";

			CDboTSActTWaitTS
			{
				taid = 6;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 192120;
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
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 192107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 20;
			}
			CDboTSClickNPC
			{
				npcidx = "4511502;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 192109;
				ctype = 1;
				idx = 4511502;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 192108;
				area = 192101;
				goal = 192104;
				m0fz = "-2662.000000";
				m0widx = 1;
				scitem = -1;
				sort = 192105;
				stype = 16;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "3384.000000";
				m0ttip = 192115;
				rwd = 100;
				taid = 1;
				title = 192102;
				gtype = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSColRgn
			{
				widx = 1;
				cct = 1;
				crt = 0;
				r = "30.000000";
				x = "3384.000000";
				z = "-2662.000000";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 192114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 192101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511502;";
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
			cont = 192108;
			gtype = 1;
			oklnk = 2;
			area = 192101;
			goal = 192104;
			sort = 192105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 192102;
		}
	}
}

