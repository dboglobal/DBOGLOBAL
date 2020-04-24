CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 350;
	title = 35002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;";

			CDboTSActNPCConv
			{
				conv = 35022;
				ctype = 1;
				idx = 4511305;
				taid = 1;
			}
			CDboTSActTWaitTS
			{
				taid = 2;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
			CDboTSActPCConv
			{
				taid = 2;
				tblidx = 35020;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActNPCConv
			{
				conv = 35021;
				ctype = 1;
				idx = 4511305;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckInNPC
			{
				idx = 4511305;
				rad = 20;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSSkillUse
			{
				idx = 420111;
				rp = 0;
				stype = 6;
				tidx = 4511305;
				ttype = 1;
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
				conv = 35009;
				ctype = 1;
				idx = 4751401;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 35008;
				area = 35001;
				goal = 35004;
				m0fz = "-1739.000000";
				m0widx = 1;
				scitem = -1;
				sort = 35005;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "4757.000000";
				m0ttip = 35015;
				rwd = 100;
				taid = 1;
				title = 35002;
				gtype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 35014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 35001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "7;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511305;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 35007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckPCCls
			{
				clsflg = 16;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSClickNPC
			{
				npcidx = "4751401;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "351;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 35008;
			gtype = 2;
			oklnk = 2;
			area = 35001;
			goal = 35004;
			sort = 35005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 35002;
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
	}
}

