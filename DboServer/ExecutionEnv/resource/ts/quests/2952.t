CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2952;
	title = 295202;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 295220;
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
				r = "40.000000";
				x = "2909.000000";
				z = "-4030.000000";
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
				conv = 295209;
				ctype = 1;
				idx = 4752110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 295208;
				area = 295201;
				goal = 295204;
				m0fz = "-4030.000000";
				m0widx = 1;
				scitem = -1;
				sort = 295205;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "2909.000000";
				m0ttip = 295215;
				rwd = 100;
				taid = 1;
				title = 295202;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 295214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 295201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4752110;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 295207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 56;
			}
			CDboTSClickNPC
			{
				npcidx = "4752110;";
			}
			CDboTSCheckClrQst
			{
				and = "2951;";
				flink = 0;
				flinknextqid = "2953;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 295208;
			gtype = 1;
			oklnk = 2;
			area = 295201;
			goal = 295204;
			sort = 295205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 295202;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
	}
}

