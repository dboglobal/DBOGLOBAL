CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3024;
	title = 302402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 302407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 59;
			}
			CDboTSClickNPC
			{
				npcidx = "3189107;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "3025;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 302414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 302401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511519;";
			}
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
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 302408;
			gtype = 1;
			oklnk = 2;
			area = 302401;
			goal = 302404;
			sort = 302405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 302402;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 302409;
				ctype = 1;
				idx = 3189107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 302408;
				area = 302401;
				goal = 302404;
				m0fz = "-4870.259766";
				m0widx = 1;
				scitem = -1;
				sort = 302405;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "4018.189941";
				m0ttip = 302415;
				rwd = 100;
				taid = 1;
				title = 302402;
				gtype = 1;
			}
		}
	}
}

